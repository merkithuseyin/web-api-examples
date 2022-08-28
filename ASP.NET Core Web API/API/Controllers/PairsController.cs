using System.Net;
using System.Threading.Tasks;
using Application.UseCases.Pairs.DeletePair;
using Application.UseCases.Pairs.GetAllPairs;
using Application.UseCases.Pairs.GetPair;
using Application.UseCases.Pairs.SavePair;
using Application.UseCases.Pairs.UpdatePair;
using Domain.Entities;
using MediatR;
using Microsoft.AspNetCore.Mvc;

namespace API.Controllers;

[ApiController]
[Route("[controller]")]
public class PairsController : ControllerBase
{
    private readonly IMediator _mediator;
    public record NameValueRequestModel(string Name, string Value);

    public PairsController(IMediator mediator)
    {
        _mediator = mediator;
    }

    [HttpGet]
    [ProducesResponseType((int)HttpStatusCode.OK)]
    public async Task<IActionResult> GetAll()
    {
        var applicationResponse = await _mediator.Send(new GetAllPairsRequest());
        return Ok(applicationResponse.Pairs);
    }

    [HttpGet("{id:int}")]
    [ProducesResponseType((int)HttpStatusCode.OK)]
    [ProducesResponseType((int)HttpStatusCode.NotFound)]
    public async Task<IActionResult> Get(int id)
    {
        var applicationResponse = await _mediator.Send(new GetPairRequest(id));
        return applicationResponse.Pair is not null ? Ok(applicationResponse.Pair) : NotFound(); 
    }

    
    [HttpPost]
    [ProducesResponseType((int)HttpStatusCode.OK)]
    [ProducesResponseType((int)HttpStatusCode.BadRequest)]
    public async Task<IActionResult> Save([FromBody] NameValueRequestModel requestModel)
    {
        var pair = new Pair
        {
            Name = requestModel.Name,
            Value = requestModel.Value
        };
        
        var applicationResponse = await _mediator.Send(new SavePairRequest(pair));
        return applicationResponse.IsSuccess ? Ok() : BadRequest();
    }

    
    [HttpPatch("{id:int}")]
    [ProducesResponseType((int)HttpStatusCode.OK)]
    [ProducesResponseType((int)HttpStatusCode.NotFound)]
    public async Task<IActionResult> Update(int id, [FromBody] NameValueRequestModel requestModel)
    {
        var pair = new Pair
        {
            Id = id,
            Name = requestModel.Name,
            Value = requestModel.Value
        };

        var applicationResponse = await _mediator.Send(new UpdatePairRequest(id, pair));
        return applicationResponse.IsSuccess ? Ok() : NotFound(); 
    }
    
    [HttpDelete("{id:int}")]
    [ProducesResponseType((int)HttpStatusCode.OK)]
    [ProducesResponseType((int)HttpStatusCode.NotFound)]
    public async Task<IActionResult> Delete(int id)
    {
        var applicationResponse = await _mediator.Send(new DeletePairRequest(id));
        return applicationResponse.IsSuccess ? Ok() : NotFound(); 
    }
}