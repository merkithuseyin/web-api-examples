using System.Threading;
using System.Threading.Tasks;
using Domain.RepositoryInterfaces;
using MediatR;

namespace Application.UseCases.Pairs.GetAllPairs;

public class GetAllPairsHandler : IRequestHandler<GetAllPairsRequest, GetAllPairsResponse>
{
    private readonly IPairRepository _pairRepository;

    public GetAllPairsHandler(IPairRepository pairRepository)
    {
        _pairRepository = pairRepository;
    }
        
    public async Task<GetAllPairsResponse> Handle(GetAllPairsRequest request, CancellationToken cancellationToken)
    {
        request.Validate();

        var allPairs = await _pairRepository.GetAllAsync(cancellationToken);

        return new GetAllPairsResponse(allPairs);
    }
}