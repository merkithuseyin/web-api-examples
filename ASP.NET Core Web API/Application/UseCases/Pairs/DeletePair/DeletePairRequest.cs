using Application.Interfaces;
using Domain;
using MediatR;

namespace Application.UseCases.Pairs.DeletePair;

public record DeletePairRequest(int Id): IRequest<DeletePairResponse>, IRequestValidator
{
    public void Validate()
    {
        if (Id <= 0)
        {
            throw new System.ArgumentException(StringResources.Id_Can_Not_Be_Non_Positive_Number);
        }
    }
}